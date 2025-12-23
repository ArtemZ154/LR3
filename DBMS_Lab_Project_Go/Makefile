.PHONY: run build test coverage clean

# Запуск приложения
run:
	go run cmd/main.go

# Сборка исполняемого файла
build:
	go build -o dbms cmd/main.go

# Запуск тестов
test:
	go test ./...

# Проверка покрытия тестами
coverage:
	go test -coverprofile=coverage.out ./...
	go tool cover -func=coverage.out

# Открытие отчета о покрытии в браузере
coverage-html: coverage
	go tool cover -html=coverage.out

# Очистка временных файлов
clean:
	rm -f dbms coverage.out coverage.html
